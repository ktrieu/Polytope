#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <future>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "mat_parse.h"
#include "FileSystemWatcher.h"

namespace fs = std::filesystem;

fs::path get_output_path(const fs::path& output_dir, const fs::path& name) {
	fs::path out = output_dir / name;
	if (out.extension().string() == std::string(".blend")) {
		return out.replace_extension(".mdl");
	}
	if (out.extension().string() == std::string(".png")) {
		return out.replace_extension(".tex");
	}
	else {
		return out;
	}
}

void process_mdl(const fs::path& in, const fs::path& out, const std::string& name) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(in.string(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	if (scene == nullptr) {
		std::cout << "Error processing " << in << "\n";
		std::cout << "Error message: " << importer.GetErrorString() << "\n";
		return;
	}
	aiMesh* ai_mesh = scene->mMeshes[0];
	Mesh mesh;
	mesh.name = name;
	for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
		aiVector3D pos = ai_mesh->mVertices[i];
		aiVector3D normal = ai_mesh->mNormals[i];
		Vertex vertex;
		vertex.pos.x = pos.x;
		vertex.pos.y = pos.y;
		vertex.pos.z = pos.z;
		vertex.normal.x = normal.x;
		vertex.normal.y = normal.y;
		vertex.normal.z = normal.z;
		if (ai_mesh->HasTextureCoords(0)) {
			aiVector3D uv = ai_mesh->mTextureCoords[0][i];
			vertex.uv.x = uv.x;
			vertex.uv.y = uv.y;
		}
		mesh.vertexes.push_back(vertex);
	}
	for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
		aiFace ai_face = ai_mesh->mFaces[i];
		for (unsigned int j = 0; j < ai_face.mNumIndices; j++) {
			mesh.indexes.push_back(ai_face.mIndices[j]);
		}
	}
	std::ofstream ostream(out, std::ios::binary);
	cereal::BinaryOutputArchive archive(ostream);
	archive(mesh);
}

void process_mat(const fs::path& in, const fs::path& out, const std::string& name) {
	std::ifstream file(in);
	Material mat = parse_material_file(file);
	mat.name = name;
	std::ofstream out_file(out, std::ios::binary);
	cereal::BinaryOutputArchive archive(out_file);
	archive(mat);
}

void process_texture(const fs::path& in, const fs::path& out, const std::string& name) {
	Texture texture;
	texture.name = name;
	unsigned char* data = stbi_load(in.string().c_str(), &texture.width, &texture.height, &texture.channels_per_pixel, 0);
	if (data == nullptr) {
		stbi_image_free(data);
		throw std::runtime_error("Could not load image " + in.string() + ": " + stbi_failure_reason());
	}
	long data_length = (long)texture.width * (long)texture.height * (long)texture.channels_per_pixel;
	texture.data = std::vector<unsigned char>(data, data + data_length);
	stbi_image_free(data);
	std::ofstream out_file(out, std::ios::binary);
	cereal::BinaryOutputArchive archive(out_file);
	archive(texture);
}

void process_misc(const fs::path& in, const fs::path& out) {
	fs::copy(in, out, fs::copy_options::overwrite_existing);
}

void process(const fs::path& in, const fs::path& out, const std::string& name) {
	//if the output path doesn't exist, create it
	fs::create_directories(fs::path(out).remove_filename());
	if (in.extension().string() == std::string(".blend")) {
		process_mdl(in, out, name);
	}
	else if (in.extension().string() == std::string(".pmat")) {
		process_mat(in, out, name);
	}
	else if (in.extension().string() == std::string(".png")) {
		process_texture(in, out, name);
	}
	else {
		process_misc(in, out);
	}
}

void process_directory(fs::path input_dir, fs::path output_dir) {
	fs::recursive_directory_iterator resource_iter(input_dir);
	for (fs::path resource_in : resource_iter) {
		fs::path resource_name = resource_in.lexically_relative(input_dir);
		fs::path resource_out = get_output_path(output_dir, resource_name);
		if (!fs::is_regular_file(resource_in)) {
			//we only want to process files
			continue;
		}
		if (!(GetFileAttributes(resource_in.string().c_str()) & FILE_ATTRIBUTE_HIDDEN)) {
			// don't process Visual Studio temp files
			continue;
		}
		if (!fs::is_regular_file(resource_out) || fs::last_write_time(resource_in) > fs::last_write_time(resource_out)) {
			std::cout << "Processing " << resource_name.generic_string() << "\n";
			process(resource_in, resource_out, resource_name.replace_extension().generic_string());
		}
	}
}

static void wait_for_input() {
	char c;
	std::cin.get(c);
	return;
}

const int WAIT_TIMEOUT_MS = 100;

int main(int argc, char** argv) {
	fs::path input_dir;
	fs::path output_dir;
	if (argc == 3) {
		input_dir = fs::path(argv[1]);
		output_dir = fs::path(argv[2]);
	}
	else {
		std::cout << "Invalid number of arguments.\n";
		std::cout << "Usage: PolyTopeTools inputDir outputDir\n";
		return 1;
	}
	std::cout << "PolytopeTools:\n";
	std::cout << "Writing resources to " << output_dir << "\n";
	if (!fs::is_directory(output_dir)) {
		std::error_code ec;
		//attempt to create the output directory first
		if (!fs::create_directory(output_dir, ec) || ec.value() != 0) {
			std::cout << "Error: " << output_dir << " is not a valid directory.\n";
			std::cout << "OS error code: " << ec << "\n";
			return 0;
		}
	}

	FileSystemWatcher watcher(fs::absolute(input_dir).string());
	auto future = std::async(wait_for_input);

	process_directory(input_dir, output_dir);

	std::cout << "Watching for changes...\n";
	std::cout << "Press <ENTER> to exit.\n";

	while (future.wait_for(std::chrono::milliseconds(std::chrono::milliseconds(0))) != std::future_status::ready) {
		if (watcher.check_changes()) {
			process_directory(input_dir, output_dir);
			std::cout << "Watching for changes...\n";
			std::cout << "Press <ENTER> to exit.\n";
		}
	}
	
	return 0;
}